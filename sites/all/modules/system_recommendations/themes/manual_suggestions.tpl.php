<div class="cmt-suggestion">
    <?php
    $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
    if(!empty($suggestions)){
    foreach($suggestions as $item){
        $comment_wrapper_class = "comment ajax-comment-wrapper clearfix comment-wrapper-".$item['id'];
     ?>
    <div class="<?php print $comment_wrapper_class;?>">
        <div class="suggestion-right-box" href="<?php print $item['redirect_url'];?>">
        	<a href="<?php print $item['redirect_url'];?>" rel="nofollow" class="buyButton suggestion-outclick" target="_blank" onClick="_gaq.push(['_trackEvent', 'Wish_detail_buy', 'Click', 'Buy_<?php print $baseurl;?>']);"></a>
            <div class="left-box">
                <div class="field field-name-field-suggestion-image field-type-image field-label-hidden">
                    <div class="field-items">
                        <div class="field-item odd"><img width="360" height="480" alt="" src="<?php print $item['img_url'];?>"></div>
                    </div>
                </div>
            </div>
            <div class="right-box">
                <div class="right-box-text1">
                    <div class="titletext jquery-once-5-processed">
                        <div class="field field-name-field-suggestion-title field-type-text field-label-hidden">
                            <div class="field-items">
                                <div class="field-item odd"><?php print $item['name'];?></div>
                            </div>
                        </div>
                        <span class="img-more" style="display: inline;"></span>
                        <div class="title-fulltext">
                            <div class="field field-name-field-suggestion-title field-type-text field-label-hidden">
                                <div class="field-items">
                                    <div class="field-item odd"><?php print $item['name'];?></div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="price-listtext">
                        Price <?php print $item['price_unit'];?>:
                        <div class="field field-name-field-suggestion-price field-type-number-decimal field-label-hidden">
                            <div class="field-items">
                                <div class="field-item odd"><?php print $item['price'];?></div>
                            </div>
                        </div>
                    </div>
                    <div class="field field-name-field-suggestion-source field-type-link-field field-label-hidden">
                        <div class="field-items">
                            <div class="field-item odd">
                                <a rel="nofollow" class="form-group form-groupform-groupform-groupform-groupform-groupform-group buyButton" target="_blank" href="<?php print $item['redirect_url'];?>" onClick="_gaq.push(['_trackEvent', 'Wish_detail_buy', 'Click', 'Buy_<?php print $origin_url_base;?>']);">
                                <?php
								//$origin_url_base='';
                                if(isset($item['origin_url_base'])) {
                                    print $item['origin_url_base'];
									//$origin_url_base=$item['origin_url_base'];
                                } else {
                                    print $item['origin_url_base_path'];
									//$origin_url_base=$item['origin_url_base']['host'];
                                }
                                ?></a>
                            </div>
                        </div>
                    </div>
                    <div class="like-button">
                        <?php
                            $widget_code = rate_generate_widget(1, 'comment', $item['id']);
                            print $widget_code;
                        ?>
                    </div>
                    <div class="clock-button">
                    <?php
                    $cdate = strtotime($item['cdate']);
                    print gmdate('d/m/Y',$cdate);?></div>
                        <div class="comment_link">
                            <ul class="links inline">
                                <?php if(!empty($item['comment_edit_link'])){ ?>
                                    <li><?php print l('Edit', $item['comment_edit_link'], array('attributes' => array('class' => 'use-ajax ajax-comment-edit ajax-processed')));?></li>
                                <?php } ?>
                                <?php if(!empty($item['comment_delete_link'])){ ?>
                                    <li><?php print l('Delete', $item['comment_delete_link'], array('attributes' => array('class' => 'use-ajax ajax-processed')));?></li>
                                <?php } ?>
                            </ul>
                        </div>
                </div>
                <div class="right-box-text2">
                      <div class="user-picture">
                        <?php
                            $user = user_load($item['uid']);
                            $user_name = $user->name;
                            $user_picture = file_create_url($user->picture->uri);
                            $user_picture = parse_url($user_picture);
                            $user_picture_path = $user_picture['path'];
                        ?>
                        <img title="<?php print $user_name;?>'s picture" alt="<?php print $user_name;?>'s picture" src="<?php print $user_picture_path;?>">
                      </div>
                      <span class="tooltipsle">
                        <a href="<?php print $item['redirect_url'];?>" rel="nofollow" class="buyButton" target="_blank" onClick="_gaq.push(['_trackEvent', 'Wish_detail_buy', 'Click', 'Buy_<?php print $item['origin_url_base'];?>']);" >Shop</a>
                        <!--span class="tltp">Check more details and buy from partner site</span-->
                      </span>
                </div>
            </div>
        </div>
    </div>
    <?php
        }
    }
    ?>
</div>
